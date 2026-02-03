"""
Module for the Memory tab of the Resource Monitor.

Displays real-time RAM and Swap usage statistics and graphs.
"""
import psutil
import tkinter as tk
from tkinter import ttk
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import collections
from .scrollable_tab import ScrollableTab
from .utils import ExportButton, apply_plot_theme, get_size

class MemoryTab(ScrollableTab):
    """A Tkinter Frame for displaying Memory information and real-time usage graphs."""

    def __init__(self, parent):
        """Initialize the MemoryTab UI components."""
        super().__init__(parent)

        self.mem_data = collections.deque(maxlen=50)

        self.mem_fig = Figure(figsize=(5, 3), dpi=100)
        self.mem_ax = self.mem_fig.add_subplot(111)
        self.mem_ax.set_title("RAM Usage")
        self.mem_ax.set_ylim(0, 100)
        self.mem_line, = self.mem_ax.plot([], [])
        self.mem_canvas = FigureCanvasTkAgg(self.mem_fig, self.scrollable_frame)
        self.mem_canvas.get_tk_widget().pack(pady=5, fill='both', expand=True)

        btn_frame = ttk.Frame(self.scrollable_frame)
        btn_frame.pack(fill='x', padx=10)
        ExportButton(btn_frame, self.mem_fig, self).pack(side='right')

        self.mem_info_frame = ttk.LabelFrame(self.scrollable_frame, text="Memory Information")
        self.mem_info_frame.pack(pady=5, padx=10, fill='x')
        self.mem_info_frame.bind("<Configure>", self.on_resize)
        self.layout_mode = None
        
        self.ram_frame = ttk.LabelFrame(self.mem_info_frame, text="RAM")
        # Initial pack handled by rearrange_frames

        # RAM Grid Layout
        ttk.Label(self.ram_frame, text="Total:").grid(row=0, column=0, sticky='w', padx=5, pady=2)
        self.ram_total_val = ttk.Label(self.ram_frame, text="")
        self.ram_total_val.grid(row=0, column=1, sticky='w', padx=5, pady=2)

        ttk.Label(self.ram_frame, text="Available:").grid(row=1, column=0, sticky='w', padx=5, pady=2)
        self.ram_available_val = ttk.Label(self.ram_frame, text="")
        self.ram_available_val.grid(row=1, column=1, sticky='w', padx=5, pady=2) 

        ttk.Label(self.ram_frame, text="Used:").grid(row=2, column=0, sticky='w', padx=5, pady=2)
        self.ram_used_val = ttk.Label(self.ram_frame, text="")
        self.ram_used_val.grid(row=2, column=1, sticky='w', padx=5, pady=2)

        ttk.Label(self.ram_frame, text="Free:").grid(row=3, column=0, sticky='w', padx=5, pady=2)
        self.ram_free_val = ttk.Label(self.ram_frame, text="")
        self.ram_free_val.grid(row=3, column=1, sticky='w', padx=5, pady=2)
        
        self.swap_mem_frame = ttk.LabelFrame(self.mem_info_frame, text="Swap Memory")
        # Initial pack handled by rearrange_frames

        # Swap Grid Layout
        ttk.Label(self.swap_mem_frame, text="Total:").grid(row=0, column=0, sticky='w', padx=5, pady=2)
        self.swap_total_val = ttk.Label(self.swap_mem_frame, text="")
        self.swap_total_val.grid(row=0, column=1, sticky='w', padx=5, pady=2)

        ttk.Label(self.swap_mem_frame, text="Used:").grid(row=1, column=0, sticky='w', padx=5, pady=2)
        self.swap_used_val = ttk.Label(self.swap_mem_frame, text="")
        self.swap_used_val.grid(row=1, column=1, sticky='w', padx=5, pady=2)

        ttk.Label(self.swap_mem_frame, text="Free:").grid(row=2, column=0, sticky='w', padx=5, pady=2)
        self.swap_free_val = ttk.Label(self.swap_mem_frame, text="")
        self.swap_free_val.grid(row=2, column=1, sticky='w', padx=5, pady=2)

        ttk.Label(self.swap_mem_frame, text="Percent:").grid(row=3, column=0, sticky='w', padx=5, pady=2)
        self.swap_percent_val = ttk.Label(self.swap_mem_frame, text="")
        self.swap_percent_val.grid(row=3, column=1, sticky='w', padx=5, pady=2)
        
        self.rearrange_frames(vertical=False)

    def on_resize(self, event):
        """Handle the resize event to switch between vertical and horizontal layouts."""
        if event.width < 10: return
        if event.width < 600:
            if self.layout_mode != 'v':
                self.rearrange_frames(vertical=True)
        else:
            if self.layout_mode != 'h':
                self.rearrange_frames(vertical=False)

    def rearrange_frames(self, vertical):
        """
        Rearrange the RAM and Swap frames to be vertical or horizontal.

        Args:
            vertical (bool): If True, stack frames vertically. Otherwise, place side-by-side.

        """
        self.ram_frame.pack_forget()
        self.swap_mem_frame.pack_forget()
        
        if vertical:
            self.ram_frame.pack(pady=5, padx=10, fill='x', side='top')
            self.swap_mem_frame.pack(pady=5, padx=10, fill='x', side='top')
            self.layout_mode = 'v'
        else:
            self.ram_frame.pack(pady=5, padx=10, fill='x', side='left', expand=True)
            self.swap_mem_frame.pack(pady=5, padx=10, fill='x', side='left', expand=True)
            self.layout_mode = 'h'

    def fetch_data(self):
        """
        Gather current Memory statistics.

        Returns:
            dict: A dictionary containing mem (virtual_memory) and swap (swap_memory) objects.
        """
        mem = psutil.virtual_memory()
        swap = psutil.swap_memory()
        return {
            "mem": mem,
            "swap": swap
        }

    def update_ui(self, data):
        """
        Update the UI with the latest Memory data.

        Args:
            data (dict): The data dictionary returned by fetch_data.
        """
        mem = data["mem"]
        self.mem_data.append(mem.percent)
        self.mem_line.set_data(range(len(self.mem_data)), self.mem_data)
        self.mem_ax.relim()
        self.mem_ax.autoscale_view()
        
        if self.winfo_viewable():
            self.mem_fig.canvas.draw()
        
        self.ram_total_val.config(text=f"{get_size(mem.total)}")
        self.ram_available_val.config(text=f"{get_size(mem.available)}")
        self.ram_used_val.config(text=f"{get_size(mem.used)}")
        self.ram_free_val.config(text=f"{get_size(mem.free)}")
        
        swap = data["swap"]
        self.swap_total_val.config(text=f"{get_size(swap.total)}")
        self.swap_used_val.config(text=f"{get_size(swap.used)}")
        self.swap_free_val.config(text=f"{get_size(swap.free)}")
        self.swap_percent_val.config(text=f"{swap.percent}%")

    def set_theme(self, theme_data):
        """Update the tab theme."""
        super().set_theme(theme_data)
        apply_plot_theme(self.mem_fig, self.mem_ax, theme_data)
        self.mem_canvas.draw()
