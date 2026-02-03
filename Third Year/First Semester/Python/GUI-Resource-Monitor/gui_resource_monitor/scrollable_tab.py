"""
Module for a reusable scrollable tab base class.
"""
import tkinter as tk
from tkinter import ttk

class ScrollableTab(ttk.Frame):
    """A reusable base class for creating a tab with a vertical scrollbar."""

    def __init__(self, parent, *args, **kwargs):
        """Initialize the scrollable frame structure."""
        super().__init__(parent, *args, **kwargs)

        self.grid_rowconfigure(0, weight=1)
        self.grid_columnconfigure(0, weight=1)

        # Scrollable setup
        self.main_canvas = tk.Canvas(self, highlightthickness=0)
        self.scrollbar = ttk.Scrollbar(self, orient="vertical", command=self.main_canvas.yview)
        
        # The scrollable_frame is where subclasses will place their widgets.
        self.scrollable_frame = ttk.Frame(self.main_canvas, padding=15)

        self.scrollable_frame.bind("<Configure>", self._update_scroll_region)

        self.canvas_window = self.main_canvas.create_window((0, 0), window=self.scrollable_frame, anchor="nw")
        self.main_canvas.configure(yscrollcommand=self.scrollbar.set)

        self.main_canvas.grid(row=0, column=0, sticky="nsew")
        self.main_canvas.bind("<Configure>", self._on_canvas_configure)

    def _update_scroll_region(self, event=None):
        """Update the scroll region to encompass all widgets in the scrollable_frame."""
        self.main_canvas.configure(scrollregion=self.main_canvas.bbox("all"))
        self._check_scroll_needed()

    def _on_canvas_configure(self, event):
        """Adjust the width of the scrollable_frame to match the canvas and check for scrollbar."""
        self.main_canvas.itemconfig(self.canvas_window, width=event.width, height=max(event.height, self.scrollable_frame.winfo_reqheight()))
        self._check_scroll_needed()

    def _check_scroll_needed(self):
        """Show or hide the scrollbar based on content height."""
        bbox = self.main_canvas.bbox("all")
        if not bbox: return

        if bbox[3] > self.main_canvas.winfo_height():
            self.scrollbar.grid(row=0, column=1, sticky="ns")
        else:
            self.scrollbar.grid_remove()
            
    def set_theme(self, theme_data):
        """Update the base theme for the canvas background."""
        colors = theme_data['colors']
        self.main_canvas.configure(bg=colors['bg'])