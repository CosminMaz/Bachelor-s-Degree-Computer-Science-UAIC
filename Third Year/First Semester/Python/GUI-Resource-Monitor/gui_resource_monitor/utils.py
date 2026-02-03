"""
Utility functions for the Resource Monitor.
"""
import os
import sys
from tkinter import filedialog, messagebox, ttk

def get_size(bytes, suffix="B"):
    """
    Convert bytes to a human-readable string format (e.g., 10.5MB).

    Args:
        bytes (float): The size in bytes.
        suffix (str): The suffix to append (default is "B").

    Returns:
        str: Human-readable size string.
    """
    factor = 1024
    for unit in ["", "K", "M", "G", "T", "P"]:
        if bytes < factor:
            return f"{bytes:.2f}{unit}{suffix}"
        bytes /= factor
    return f"{bytes:.2f}{unit}{suffix}"

def get_drive_type(device_name):
    """
    Determine the drive type (SSD or HDD) for a given device name.

    Args:
        device_name (str): The name of the device (e.g., "sda").

    Returns:
        str: "SSD", "HDD", or "N/A" if undetermined.
    """
    try:
        # device_name can be a partition like "sda1" or a disk like "sda"
        
        # If it's a partition, find the parent disk
        pkname_path = f'/sys/class/block/{device_name}/pkname'
        if os.path.exists(pkname_path):
            with open(pkname_path, 'r') as f:
                disk_name = f.read().strip()
        else:
            disk_name = device_name
        
        if 'nvme' in disk_name:
            return 'SSD'

        rotational_file = f'/sys/class/block/{disk_name}/queue/rotational'

        if not os.path.exists(rotational_file):
            return 'N/A' # For virtual devices

        with open(rotational_file, 'r') as f:
            if f.read().strip() == '0':
                return 'SSD'
            else:
                return 'HDD'
    except (IOError, OSError, FileNotFoundError):
        return 'N/A'

def export_plot(figure, parent):
    """
    Export the current matplotlib figure to a file.

    Args:
        figure (matplotlib.figure.Figure): The figure to save.
        parent (tk.Widget): The parent widget for the dialog.
    """
    try:
        file_path = filedialog.asksaveasfilename(
            parent=parent,
            defaultextension=".pdf",
            filetypes=[("PDF Document", "*.pdf"), ("JPEG Image", "*.jpg"), ("PNG Image", "*.png"), ("All Files", "*.*")],
            title="Export Graph"
        )
        if file_path:
            figure.savefig(file_path)
            messagebox.showinfo("Export Successful", f"Graph saved to {file_path}")
    except Exception as e:
        messagebox.showerror("Export Failed", f"An error occurred: {e}")

def apply_plot_theme(figure, ax, theme_data):
    """
    Apply light or dark theme to a matplotlib plot.

    Args:
        figure: The matplotlib figure object.
        ax: The matplotlib axes object.
        theme_data (dict): The theme dictionary containing colors.
    """
    colors = theme_data['colors']
    bg = colors['bg']
    fg = colors['fg']

    figure.patch.set_facecolor(bg)
    ax.set_facecolor(bg)
    ax.title.set_color(fg)
    ax.xaxis.label.set_color(fg)
    ax.yaxis.label.set_color(fg)
    ax.tick_params(axis='x', colors=fg)
    ax.tick_params(axis='y', colors=fg)
    for spine in ax.spines.values():
        spine.set_color(fg)
    
    legend = ax.get_legend()
    if legend:
        legend.get_frame().set_facecolor(bg)
        legend.get_frame().set_edgecolor(fg)
        for text in legend.get_texts():
            text.set_color(fg)

class ExportButton(ttk.Button):
    """A reusable 'Export Graph' button."""
    def __init__(self, parent, figure, dialog_parent, *args, **kwargs):
        """
        Initialize the ExportButton.

        Args:
            parent (tk.Widget): The parent widget for this button.
            figure (matplotlib.figure.Figure): The figure to be exported.
            dialog_parent (tk.Widget): The parent widget for the export dialog.
        """
        # The command calls the existing export_plot function
        super().__init__(parent, text="Export Graph", command=lambda: export_plot(figure, dialog_parent), *args, **kwargs)
