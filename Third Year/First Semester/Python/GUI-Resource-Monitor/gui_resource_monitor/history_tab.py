"""
Module for the History tab of the Resource Monitor.

This module handles the visualization of historical resource usage data retrieved from the database using Matplotlib.
"""
import tkinter as tk
from tkinter import ttk
import sqlite3
import warnings
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
import datetime
from .scrollable_tab import ScrollableTab
from .utils import ExportButton, apply_plot_theme, get_size

class XAxisNavigationToolbar(NavigationToolbar2Tk):
    """Custom Matplotlib navigation toolbar that restricts panning and zooming to the X-axis."""

    # Rename Home button to Reset
    toolitems = [t if t[0] != 'Home' else ('Reset', 'Reset original view', 'home', 'home') 
                 for t in NavigationToolbar2Tk.toolitems]

    def drag_pan(self, event):
        """Restrict panning to the X-axis."""
        event.key = 'x'
        super().drag_pan(event)

    def drag_zoom(self, event):
        """Restrict zooming to the X-axis."""
        event.key = 'x'
        super().drag_zoom(event)

class HistoryTab(ScrollableTab):
    """A Tkinter Frame that displays historical resource usage charts."""

    def __init__(self, parent):
        """Initialize the HistoryTab UI components."""
        super().__init__(parent)

        # Initialize the figure before it's used by the export button
        self.fig = Figure(figsize=(10, 6), dpi=100)

        self.db_file = "resource_monitor.db"
        self.current_theme_data = None
        # Create a frame for the filter controls
        filter_frame = ttk.LabelFrame(self.scrollable_frame, text="Time Range Selection")
        filter_frame.pack(side=tk.TOP, fill=tk.X, padx=10, pady=5, expand=True)

        # Presets
        presets_frame = ttk.Frame(filter_frame)
        presets_frame.pack(side=tk.TOP, fill=tk.X, padx=5, pady=5)
        
        ttk.Label(presets_frame, text="Presets:").pack(side=tk.LEFT, padx=(0, 5))
        
        presets = [
            ("1 Hour", 1),
            ("6 Hours", 6),
            ("24 Hours", 24),
            ("7 Days", 168)
        ]
        
        for text, hours in presets:
            ttk.Button(presets_frame, text=text, command=lambda h=hours: self.apply_preset(h)).pack(side=tk.LEFT, padx=2)

        # Custom Range
        custom_frame = ttk.Frame(filter_frame)
        custom_frame.pack(side=tk.TOP, fill=tk.X, padx=5, pady=5)

        # Start time
        ttk.Label(custom_frame, text="Start (YYYY-MM-DD HH:MM):").pack(side=tk.LEFT, padx=(0, 5))
        self.start_time_entry = ttk.Entry(custom_frame)
        self.start_time_entry.pack(side=tk.LEFT)

        # End time
        ttk.Label(custom_frame, text="End (YYYY-MM-DD HH:MM):").pack(side=tk.LEFT, padx=(10, 5))
        self.end_time_entry = ttk.Entry(custom_frame)
        self.end_time_entry.pack(side=tk.LEFT)

        # Refresh button
        refresh_button = ttk.Button(custom_frame, text="Apply Custom", command=self.update_charts)
        refresh_button.pack(side=tk.LEFT, padx=(10, 0))

        # Export button
        ExportButton(custom_frame, self.fig, self).pack(side=tk.LEFT, padx=(10, 0))

        # View Selection
        view_frame = ttk.Frame(filter_frame)
        view_frame.pack(side=tk.TOP, fill=tk.X, padx=5, pady=5)
        
        ttk.Label(view_frame, text="Show:").pack(side=tk.LEFT, padx=(0, 5))
        
        self.show_cpu = tk.BooleanVar(value=True)
        self.show_mem = tk.BooleanVar(value=True)
        self.show_disk = tk.BooleanVar(value=True)
        self.show_net = tk.BooleanVar(value=True)
        
        ttk.Checkbutton(view_frame, text="CPU", variable=self.show_cpu, command=self.update_charts).pack(side=tk.LEFT, padx=5)
        ttk.Checkbutton(view_frame, text="Memory", variable=self.show_mem, command=self.update_charts).pack(side=tk.LEFT, padx=5)
        ttk.Checkbutton(view_frame, text="Disk", variable=self.show_disk, command=self.update_charts).pack(side=tk.LEFT, padx=5)
        ttk.Checkbutton(view_frame, text="Network", variable=self.show_net, command=self.update_charts).pack(side=tk.LEFT, padx=5)

        # Initialize with 1 hour
        self.apply_preset(1, update=False)

        self.canvas = FigureCanvasTkAgg(self.fig, self.scrollable_frame)

        toolbar = XAxisNavigationToolbar(self.canvas, self.scrollable_frame)
        toolbar.update()
        toolbar.pack(side=tk.TOP, fill=tk.X)

        self.canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=True)

        self.update_charts()

    def apply_preset(self, hours, update=True):
        """
        Apply a time range preset.

        Args:
            hours (int): The number of hours to look back.
            update (bool): Whether to trigger a chart update immediately.
        """
        end_time = datetime.datetime.now()
        start_time = end_time - datetime.timedelta(hours=hours)
        
        self.start_time_entry.delete(0, tk.END)
        self.start_time_entry.insert(0, start_time.strftime("%Y-%m-%d %H:%M"))
        
        self.end_time_entry.delete(0, tk.END)
        self.end_time_entry.insert(0, end_time.strftime("%Y-%m-%d %H:%M"))
        
        if update:
            self.update_charts()

    def fetch_history(self, table_name, time_column, value_columns, start_time=None, end_time=None):
        """
        Fetch historical data from the database.

        Args:
            table_name (str): The name of the database table.
            time_column (str): The name of the timestamp column.
            value_columns (list): List of column names to retrieve values for.
            start_time (float, optional): Start timestamp.
            end_time (float, optional): End timestamp.

        Returns:
            tuple: A tuple containing a list of timestamps and a list of value rows.
        """
        conn = sqlite3.connect(self.db_file)
        c = conn.cursor()

        if start_time and end_time:
            query = f"SELECT {time_column}, {', '.join(value_columns)} FROM {table_name} WHERE {time_column} BETWEEN ? AND ? ORDER BY {time_column} ASC"
            c.execute(query, (start_time, end_time))
        else:
            # Fetch last 1000 points if no time range
            query = f"SELECT {time_column}, {', '.join(value_columns)} FROM {table_name} ORDER BY {time_column} DESC LIMIT 1000"
            c.execute(query)
        
        data = c.fetchall()
        conn.close()

        if not start_time and not end_time:
            # Reverse data to have chronological order if we fetched DESC
            data.reverse()
        
        timestamps = [datetime.datetime.fromtimestamp(row[0]) for row in data]
        values = [row[1:] for row in data]
        
        return timestamps, values

    def update_charts(self):
        """Fetch data based on current filters and update the visible charts."""
        try:
            start_str = self.start_time_entry.get()
            end_str = self.end_time_entry.get()

            start_time = datetime.datetime.strptime(start_str, "%Y-%m-%d %H:%M").timestamp()
            end_time = datetime.datetime.strptime(end_str, "%Y-%m-%d %H:%M").timestamp()
        except ValueError:
            # Handle incorrect date format gracefully
            print("Invalid date format. Please use YYYY-MM-DD HH:MM")
            # Optionally, show an error message to the user in the UI
            start_time, end_time = None, None

        self.fig.clear()
        
        active_plots = []
        if self.show_cpu.get(): active_plots.append('cpu')
        if self.show_mem.get(): active_plots.append('mem')
        if self.show_disk.get(): active_plots.append('disk')
        if self.show_net.get(): active_plots.append('net')
        
        n = len(active_plots)
        if n == 0:
            self.canvas.draw()
            return

        cols = 2 if n > 1 else 1
        rows = (n + cols - 1) // cols
        
        for i, plot_type in enumerate(active_plots):
            ax = self.fig.add_subplot(rows, cols, i+1)
            if plot_type == 'cpu':
                self._plot_cpu(ax, start_time, end_time)
            elif plot_type == 'mem':
                self._plot_mem(ax, start_time, end_time)
            elif plot_type == 'disk':
                self._plot_disk(ax, start_time, end_time)
            elif plot_type == 'net':
                self._plot_net(ax, start_time, end_time)
        
        self._apply_theme_to_all()
        with warnings.catch_warnings():
            warnings.filterwarnings("ignore", message="Tight layout not applied")
            self.fig.tight_layout(pad=3.0)
        self.canvas.draw()

    def _plot_cpu(self, ax, start_time, end_time):
        """Plot CPU usage history on the given axes."""
        ts, vals = self.fetch_history("cpu_history", "timestamp", ["cpu_percent"], start_time, end_time)
        if ts:
            values = [v[0] for v in vals]
            ax.plot(ts, values)
            avg = sum(values) / len(values)
            ax.set_title(f"CPU Usage History (%) - Avg: {avg:.1f}%")
        else:
            ax.set_title("CPU Usage History (%)")
        ax.tick_params(axis='x', rotation=45)

    def _plot_mem(self, ax, start_time, end_time):
        """Plot Memory usage history on the given axes."""
        ts, vals = self.fetch_history("memory_history", "timestamp", ["mem_percent"], start_time, end_time)
        if ts:
            values = [v[0] for v in vals]
            ax.plot(ts, values)
            avg = sum(values) / len(values)
            ax.set_title(f"Memory Usage History (%) - Avg: {avg:.1f}%")
        else:
            ax.set_title("Memory Usage History (%)")
        ax.tick_params(axis='x', rotation=45)

    def _plot_disk(self, ax, start_time, end_time):
        """Plot Disk I/O history on the given axes."""
        ts, vals = self.fetch_history("disk_io_history", "timestamp", ["read_bytes", "write_bytes"], start_time, end_time)
        if ts:
            reads = [v[0] for v in vals]
            writes = [v[1] for v in vals]
            ax.plot(ts, reads, label="Read")
            ax.plot(ts, writes, label="Write")
            avg_read = sum(reads) / len(reads)
            avg_write = sum(writes) / len(writes)
            ax.set_title(f"Disk I/O (Bytes) - Avg R: {get_size(avg_read)}, W: {get_size(avg_write)}")
            ax.legend()
        else:
            ax.set_title("Disk I/O History (Bytes)")
        ax.tick_params(axis='x', rotation=45)

    def _plot_net(self, ax, start_time, end_time):
        """Plot Network I/O history on the given axes."""
        ts, vals = self.fetch_history("network_io_history", "timestamp", ["bytes_sent", "bytes_recv"], start_time, end_time)
        if ts:
            sent = [v[0] for v in vals]
            recv = [v[1] for v in vals]
            ax.plot(ts, sent, label="Sent")
            ax.plot(ts, recv, label="Received")
            avg_sent = sum(sent) / len(sent)
            avg_recv = sum(recv) / len(recv)
            ax.set_title(f"Network I/O (Bytes) - Avg S: {get_size(avg_sent)}, R: {get_size(avg_recv)}")
            ax.legend()
        else:
            ax.set_title("Network I/O History (Bytes)")
        ax.tick_params(axis='x', rotation=45)

    def _apply_theme_to_all(self):
        """Apply the current theme to all axes in the figure."""
        for ax in self.fig.axes:
            if self.current_theme_data:
                apply_plot_theme(self.fig, ax, self.current_theme_data)

    def set_theme(self, theme_data):
        """Update the tab theme."""
        super().set_theme(theme_data)
        self.current_theme_data = theme_data
        self.update_charts()
