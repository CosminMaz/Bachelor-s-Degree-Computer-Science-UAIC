"""
Module for the CPU tab of the Resource Monitor.

Displays real-time CPU usage, frequency, temperature, and per-core utilization.
"""
import psutil
import tkinter as tk
from tkinter import ttk
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import collections
import cpuinfo
from .scrollable_tab import ScrollableTab
from .utils import ExportButton, apply_plot_theme

class CPUTab(ScrollableTab):
    """A Tkinter Frame for displaying CPU information and real-time usage graphs."""

    def __init__(self, parent):
        """Initialize the CPUTab UI components."""
        super().__init__(parent)

        self.cpu_data = collections.deque(maxlen=50)

        self.cpu_fig = Figure(figsize=(5, 3), dpi=100)
        self.cpu_ax = self.cpu_fig.add_subplot(111)
        self.cpu_ax.set_title("CPU Usage")
        self.cpu_ax.set_ylim(0, 100)
        self.cpu_line, = self.cpu_ax.plot([], [])
        self.cpu_canvas = FigureCanvasTkAgg(self.cpu_fig, self.scrollable_frame)
        self.cpu_canvas.get_tk_widget().pack(pady=5, fill='both', expand=True)

        # Export button
        btn_frame = ttk.Frame(self.scrollable_frame)
        btn_frame.pack(fill='x', padx=10)
        ExportButton(btn_frame, self.cpu_fig, self).pack(side='right')

        self.cpu_info_frame = ttk.LabelFrame(self.scrollable_frame, text="CPU Information")
        self.cpu_info_frame.pack(pady=5, padx=10, fill='x')
        
        self.cpu_name_label = ttk.Label(self.cpu_info_frame, text=f"Name: {cpuinfo.get_cpu_info()['brand_raw']}")
        self.cpu_name_label.pack(anchor='w')

        self.cpu_cores_label = ttk.Label(self.cpu_info_frame, text=f"Physical Cores: {psutil.cpu_count(logical=False)}")
        self.cpu_cores_label.pack(anchor='w')
        
        self.cpu_logical_cores_label = ttk.Label(self.cpu_info_frame, text=f"Logical Cores: {psutil.cpu_count(logical=True)}")
        self.cpu_logical_cores_label.pack(anchor='w')

        self.cpu_freq_label = ttk.Label(self.cpu_info_frame, text="Frequency:")
        self.cpu_freq_label.pack(anchor='w')

        self.cpu_temp_label = ttk.Label(self.cpu_info_frame, text="Temperature:")
        self.cpu_temp_label.pack(anchor='w')
        
        self.per_cpu_frame = ttk.LabelFrame(self.scrollable_frame, text="Per-Core Usage")
        self.per_cpu_frame.pack(pady=5, padx=10, fill='x')
        self.per_cpu_frame.bind("<Configure>", self.on_resize)
        self.last_cols = 0

        self.cpu_thread_labels = []
        self.cpu_thread_progress = []
        
        style = ttk.Style()
        style.configure("Thick.Horizontal.TProgressbar", thickness=20)
        
        for i in range(psutil.cpu_count()):
            label = ttk.Label(self.per_cpu_frame, text=f"CPU {i}:")
            self.cpu_thread_labels.append(label)

            progress = ttk.Progressbar(self.per_cpu_frame, length=200, mode='determinate', style="Thick.Horizontal.TProgressbar")
            self.cpu_thread_progress.append(progress)
            
        self.rearrange_cores(1)

    def on_resize(self, event):
        """Handle the resize event for the per-core frame to adjust column count."""
        if event.width < 10: return
        # Approx width: Label (~60) + Progress (200) + Padding (~20) = 280
        cols = max(1, event.width // 280)
        if cols != self.last_cols:
            self.rearrange_cores(cols)
            self.last_cols = cols

    def rearrange_cores(self, cols):
        """
        Rearrange the per-core usage widgets into a grid.

        Args:
            cols (int): The number of columns to arrange the widgets in.

        """
        for i, (label, progress) in enumerate(zip(self.cpu_thread_labels, self.cpu_thread_progress)):
            row = i // cols
            col = (i % cols) * 2
            label.grid(row=row, column=col, sticky='w', padx=5, pady=2)
            progress.grid(row=row, column=col+1, sticky='w', padx=5, pady=2)

    def fetch_data(self):
        """
        Gather current CPU statistics.

        Returns:
            dict: A dictionary containing cpu_percent, cpu_freq, per_cpu, and temp_str.
        """
        # Using a small interval makes the call blocking but more reliable on some platforms (like VMs on Windows)
        # where non-blocking calls might consistently return 0.
        # We get per-CPU stats and calculate the overall percentage from that.
        per_cpu = psutil.cpu_percent(interval=0.1, percpu=True)
        cpu_percent = sum(per_cpu) / len(per_cpu) if per_cpu else 0

        cpu_freq = None
        try:
            # This can fail on some systems/VMs, so we handle it gracefully.
            cpu_freq = psutil.cpu_freq()
        except Exception:
            cpu_freq = None
        
        temp_str = "Temperature: N/A"
        if hasattr(psutil, "sensors_temperatures"):
            temps = psutil.sensors_temperatures()
            if temps:
                # Prioritize common CPU sensor names
                sensor_names = ['coretemp', 'k10temp', 'zenpower', 'cpu_thermal', 'k8temp']
                target_temps = []
                for name in sensor_names:
                    if name in temps:
                        target_temps = temps[name]
                        break
                
                # Fallback to first available if no specific CPU sensor found
                if not target_temps:
                    target_temps = next(iter(temps.values()))

                if target_temps:
                    temp_str = "Temperature: "
                    for i, temp in enumerate(target_temps):
                        if i >= 4: # Limit display to avoid overflow
                            temp_str += "..."
                            break
                        label = temp.label or f"T{i}"
                        temp_str += f"{label}: {temp.current}°C  "

        return {
            "cpu_percent": cpu_percent,
            "cpu_freq": cpu_freq,
            "per_cpu": per_cpu,
            "temp_str": temp_str
        }

    def update_ui(self, data):
        """
        Update the UI with the latest CPU data.

        Args:
            data (dict): The data dictionary returned by fetch_data.
        """
        cpu_percent = data["cpu_percent"]
        self.cpu_data.append(cpu_percent)
        self.cpu_line.set_data(range(len(self.cpu_data)), self.cpu_data)
        self.cpu_ax.relim()
        self.cpu_ax.autoscale_view()
        
        if self.winfo_viewable():
            self.cpu_fig.canvas.draw()
        
        cpu_freq = data["cpu_freq"]
        if cpu_freq:
            self.cpu_freq_label.config(text=f"Frequency: {cpu_freq.current:.2f} Mhz")
        else:
            self.cpu_freq_label.config(text="Frequency: N/A")

        self.cpu_temp_label.config(text=data["temp_str"])
        
        per_cpu = data["per_cpu"]
        for i, percent in enumerate(per_cpu):
            self.cpu_thread_labels[i].config(text=f"CPU {i}: {percent}%")
            self.cpu_thread_progress[i]['value'] = percent

    def set_theme(self, theme_data):
        """Update the tab theme."""
        super().set_theme(theme_data)
        apply_plot_theme(self.cpu_fig, self.cpu_ax, theme_data)
        self.cpu_canvas.draw()

        # Get progress bar colors from the theme data
        pbar_colors = theme_data.get('colors', {}).get('progressbar')
        if pbar_colors:
            style = ttk.Style()
            style.configure("Thick.Horizontal.TProgressbar", thickness=20, **pbar_colors)
