package org.example.shell;

import freemarker.template.Configuration;
import freemarker.template.Template;
import freemarker.template.TemplateExceptionHandler;
import org.example.model.Repository;

import java.awt.Desktop;
import java.io.File;
import java.io.FileWriter;
import java.io.Writer;
import java.util.HashMap;
import java.util.Map;

public class ReportCommand implements Command {
    private Repository repo;

    public ReportCommand(Repository repo) {
        this.repo = repo;
    }

    @Override
    public void execute(String[] args) throws Exception {
        Configuration cfg = new Configuration(Configuration.VERSION_2_3_32);
        cfg.setClassForTemplateLoading(this.getClass(), "/templates");
        cfg.setDefaultEncoding("UTF-8");
        cfg.setTemplateExceptionHandler(TemplateExceptionHandler.RETHROW_HANDLER);

        Template template = cfg.getTemplate("template.ftl");

        Map<String, Object> data = new HashMap<>();
        data.put("images", repo.getImages());

        File outFile = new File("report.html");
        try (Writer out = new FileWriter(outFile)) {
            template.process(data, out);
        }

        System.out.println("Report generated: report.html");
        if (Desktop.isDesktopSupported()) {
            Desktop.getDesktop().browse(outFile.toURI());
        }
    }
}
