package org.example.model;

import lombok.AllArgsConstructor;
import lombok.NoArgsConstructor;

import java.awt.*;
import java.io.File;
import java.util.Collection;
import java.util.Collections;
import java.util.List;

@AllArgsConstructor
@NoArgsConstructor
public class Repository {
    private List<Image> image;
    public void addImage(Image image) {
        this.image.add(image);
    }

    public void removeImage(Image image) {
        this.image.remove(image);
    }

    public void displayImage(Image image) {
        boolean verify = false;
        for (Image i : this.image) {
            if (i.equals(image)) {
                verify = true;
                File imageFile = new File(i.fileLocation());
                try {
                    Desktop desktop = Desktop.getDesktop();
                    desktop.open(imageFile);
                } catch (Exception e) {
                    System.out.println("Eroare la deschiderea fisierului : " + e.getMessage());
                }
            }
        }

        if (!verify) {
            System.out.println("Fiserul nu exista");
        }
    }

    public boolean removeImageById(String id) {
        this.image.removeIf(i -> i.fileLocation().equals(id));
        return true;
    }

    public Image getImageById(String id) {
        for (Image i : this.image) {
            if (i.fileLocation().equals(id)) {
                return i;
            }
        }
        return null;
    }

    public void setImages(List<Image> images) {
        image.addAll(images);
    }

    public Collection<Object> getImages() {
        return Collections.singleton(this.image);
    }
}
