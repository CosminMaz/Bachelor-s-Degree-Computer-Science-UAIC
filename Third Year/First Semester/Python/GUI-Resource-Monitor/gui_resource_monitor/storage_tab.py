"""
Module for the Storage tab of the Resource Monitor.

Displays real-time Disk I/O graphs and partition usage details.
"""
import psutil
import tkinter as tk
from tkinter import ttk
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import collections
import time
import os
from .scrollable_tab import ScrollableTab
from .utils import ExportButton, apply_plot_theme, get_drive_type, get_size

class StorageTab(ScrollableTab):
    """A Tkinter Frame for displaying Storage information and real-time I/O graphs."""

    def __init__(self, parent):
        """Initialize the StorageTab UI components."""
        super().__init__(parent)

        self.disk_read_data = collections.deque(maxlen=50)
        self.disk_write_data = collections.deque(maxlen=50)
        
        self.disk_fig = Figure(figsize=(5, 3), dpi=100)
        self.disk_ax = self.disk_fig.add_subplot(111)
        self.disk_ax.set_title("Disk I/O")
        self.disk_ax.set_ylabel("Bytes/s")
        self.disk_read_line, = self.disk_ax.plot([], [], '-', label='Read')
        self.disk_write_line, = self.disk_ax.plot([], [], '-', label='Write')
        self.disk_ax.legend()
        self.disk_canvas = FigureCanvasTkAgg(self.disk_fig, self.scrollable_frame)
        self.disk_canvas.get_tk_widget().pack(pady=5, fill='both', expand=True)

        btn_frame = ttk.Frame(self.scrollable_frame)
        btn_frame.pack(fill='x', padx=10)
        ExportButton(btn_frame, self.disk_fig, self).pack(side='right')

        self.disk_info_frame = ttk.LabelFrame(self.scrollable_frame, text="Disk Partitions")
        self.disk_info_frame.pack(pady=5, padx=10, fill='both', expand=True)

        self.disk_tree = ttk.Treeview(self.disk_info_frame, columns=('device', 'mountpoint', 'fstype', 'total', 'used', 'free', 'percent', 'type', 'read', 'write'), show='headings')
        self.disk_tree.heading('device', text='Device')
        self.disk_tree.heading('mountpoint', text='Mountpoint')
        self.disk_tree.heading('fstype', text='Type')
        self.disk_tree.heading('total', text='Total')
        self.disk_tree.heading('used', text='Used')
        self.disk_tree.heading('free', text='Free')
        self.disk_tree.heading('percent', text='Usage')
        self.disk_tree.heading('type', text='Drive Type')
        self.disk_tree.heading('read', text='Read Speed')
        self.disk_tree.heading('write', text='Write Speed')

        self.disk_tree.column('device', width=80)
        self.disk_tree.column('mountpoint', width=100)
        self.disk_tree.column('fstype', width=60)
        self.disk_tree.column('total', width=80)
        self.disk_tree.column('used', width=80)
        self.disk_tree.column('free', width=80)
        self.disk_tree.column('percent', width=60)
        self.disk_tree.column('type', width=70)
        self.disk_tree.column('read', width=80)
        self.disk_tree.column('write', width=80)
        
        self.disk_tree.pack(side="left", fill="both", expand=True)

        self.disk_scrollbar = ttk.Scrollbar(self.disk_info_frame, orient="vertical", command=self.disk_tree.yview)
        self.disk_scrollbar.pack(side="right", fill="y")
        self.disk_tree.configure(yscrollcommand=self.disk_scrollbar.set)
        
        self.last_disk_io = psutil.disk_io_counters(perdisk=True)
        self.last_io_time = time.time()
        
    def fetch_data(self):
        """
        Gather current Disk statistics.

        Returns:
            dict: A dictionary containing partition_data, read_bytes_total, and write_bytes_total.
        """
        current_io_time = time.time()
        time_delta = current_io_time - self.last_io_time
        self.last_io_time = current_io_time

        disk_io_counters = psutil.disk_io_counters(perdisk=True)
        partitions = psutil.disk_partitions()
        
        partition_data = []
        read_bytes_total = 0
        write_bytes_total = 0

        for part in partitions:
            usage = psutil.disk_usage(part.mountpoint)
            device_name = os.path.basename(part.device)
            drive_type = get_drive_type(device_name)
            
            read_speed = 0
            write_speed = 0

            if time_delta > 0 and device_name in self.last_disk_io and device_name in disk_io_counters:
                read_bytes = disk_io_counters[device_name].read_bytes - self.last_disk_io[device_name].read_bytes
                write_bytes = disk_io_counters[device_name].write_bytes - self.last_disk_io[device_name].write_bytes
                read_speed = read_bytes / time_delta
                write_speed = write_bytes / time_delta
            
            read_bytes_total += read_speed
            write_bytes_total += write_speed

            partition_data.append({
                "device": part.device,
                "mountpoint": part.mountpoint,
                "fstype": part.fstype,
                "total": usage.total,
                "used": usage.used,
                "free": usage.free,
                "percent": usage.percent,
                "drive_type": drive_type,
                "read_speed": read_speed,
                "write_speed": write_speed,
            })

        self.last_disk_io = disk_io_counters
        
        return {
            "partition_data": partition_data,
            "read_bytes_total": read_bytes_total,
            "write_bytes_total": write_bytes_total,
        }

    def update_ui(self, data):
        """
        Update the UI with the latest Storage data.

        Args:
            data (dict): The data dictionary returned by fetch_data.
        """
        for i in self.disk_tree.get_children():
            self.disk_tree.delete(i)

        for part_data in data["partition_data"]:
            self.disk_tree.insert("", "end", values=(
                part_data["device"],
                part_data["mountpoint"],
                part_data["fstype"],
                get_size(part_data["total"]),
                get_size(part_data["used"]),
                get_size(part_data["free"]),
                f"{part_data['percent']}%",
                part_data["drive_type"],
                get_size(part_data["read_speed"]) + '/s',
                get_size(part_data["write_speed"]) + '/s'
            ))

        self.disk_read_data.append(data["read_bytes_total"])
        self.disk_write_data.append(data["write_bytes_total"])

        self.disk_read_line.set_data(range(len(self.disk_read_data)), self.disk_read_data)
        self.disk_write_line.set_data(range(len(self.disk_write_data)), self.disk_write_data)
        self.disk_ax.relim()
        self.disk_ax.autoscale_view()
        
        if self.winfo_viewable():
            self.disk_fig.canvas.draw()

    def set_theme(self, theme_data):
        """Update the tab theme."""
        super().set_theme(theme_data)
        apply_plot_theme(self.disk_fig, self.disk_ax, theme_data)
        self.disk_canvas.draw()
