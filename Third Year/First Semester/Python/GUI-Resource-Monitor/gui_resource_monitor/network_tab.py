"""
Module for the Network tab of the Resource Monitor.

Displays real-time Network I/O graphs and interface details.
"""
import psutil
import tkinter as tk
from tkinter import ttk
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import collections
from .scrollable_tab import ScrollableTab
from .utils import ExportButton, apply_plot_theme, get_size

class NetworkTab(ScrollableTab):
    """A Tkinter Frame for displaying Network information and real-time I/O graphs."""

    def __init__(self, parent):
        """Initialize the NetworkTab UI components."""
        super().__init__(parent)

        self.net_sent_data = collections.deque(maxlen=50)
        self.net_recv_data = collections.deque(maxlen=50)
        
        self.net_fig = Figure(figsize=(5, 3), dpi=100)
        self.net_ax = self.net_fig.add_subplot(111)
        self.net_ax.set_title("Network I/O")
        self.net_sent_line, = self.net_ax.plot([], [], '-', label='Sent')
        self.net_recv_line, = self.net_ax.plot([], [], '-', label='Received')
        self.net_ax.legend()
        self.net_canvas = FigureCanvasTkAgg(self.net_fig, self.scrollable_frame)
        self.net_canvas.get_tk_widget().pack(pady=5, fill='both', expand=True)

        btn_frame = ttk.Frame(self.scrollable_frame)
        btn_frame.pack(fill='x', padx=10)
        ExportButton(btn_frame, self.net_fig, self).pack(side='right')
        self.net_info_frame = ttk.LabelFrame(self.scrollable_frame, text="Network Interfaces")
        self.net_info_frame.pack(pady=5, padx=10, fill='both', expand=True)
        
        self.net_tree = ttk.Treeview(self.net_info_frame, columns=('interface', 'ip', 'mac', 'speed', 'status'), show='headings')
        self.net_tree.heading('interface', text='Interface')
        self.net_tree.heading('ip', text='IP Address')
        self.net_tree.heading('mac', text='MAC Address')
        self.net_tree.heading('speed', text='Speed')
        self.net_tree.heading('status', text='Status')
        
        self.net_scrollbar = ttk.Scrollbar(self.net_info_frame, orient="vertical", command=self.net_tree.yview)
        self.net_tree.configure(yscrollcommand=self.net_scrollbar.set)
        self.net_tree.pack(side="left", fill="both", expand=True)
        self.net_scrollbar.pack(side="right", fill="y")
        
        self.last_net_io = psutil.net_io_counters()

    def fetch_data(self):
        """
        Gather current Network statistics.

        Returns:
            dict: A dictionary containing sent_bytes, recv_bytes, and interface_data.
        """
        net_io = psutil.net_io_counters()
        sent_bytes = net_io.bytes_sent - self.last_net_io.bytes_sent
        recv_bytes = net_io.bytes_recv - self.last_net_io.bytes_recv
        self.last_net_io = net_io

        net_if_addrs = psutil.net_if_addrs()
        net_if_stats = psutil.net_if_stats()
        
        interface_data = []
        for interface, addrs in net_if_addrs.items():
            ip_addr = ""
            mac_addr = ""
            for addr in addrs:
                if addr.family == psutil.AF_LINK:
                    mac_addr = addr.address
                elif addr.family == 2: # AF_INET
                    ip_addr = addr.address

            speed = 0
            status = "down"
            if interface in net_if_stats:
                speed = net_if_stats[interface].speed
                status = "up" if net_if_stats[interface].isup else "down"

            interface_data.append({
                "interface": interface,
                "ip": ip_addr,
                "mac": mac_addr,
                "speed": speed,
                "status": status,
            })
        
        return {
            "sent_bytes": sent_bytes,
            "recv_bytes": recv_bytes,
            "interface_data": interface_data
        }

    def update_ui(self, data):
        """
        Update the UI with the latest Network data.

        Args:
            data (dict): The data dictionary returned by fetch_data.
        """
        self.net_sent_data.append(data["sent_bytes"])
        self.net_recv_data.append(data["recv_bytes"])

        self.net_sent_line.set_data(range(len(self.net_sent_data)), self.net_sent_data)
        self.net_recv_line.set_data(range(len(self.net_recv_data)), self.net_recv_data)
        self.net_ax.relim()
        self.net_ax.autoscale_view()
        
        if self.winfo_viewable():
            self.net_fig.canvas.draw()

        for i in self.net_tree.get_children():
            self.net_tree.delete(i)
        
        for if_data in data["interface_data"]:
            self.net_tree.insert("", "end", values=(
                if_data["interface"],
                if_data["ip"],
                if_data["mac"],
                f"{if_data['speed']} Mbps",
                if_data["status"]
            ))
    def set_theme(self, theme_data):
        """Update the tab theme."""
        super().set_theme(theme_data)
        apply_plot_theme(self.net_fig, self.net_ax, theme_data)
        self.net_canvas.draw()
