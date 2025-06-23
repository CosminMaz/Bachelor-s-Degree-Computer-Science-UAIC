package org.example.lab11.controllers;

import lombok.AllArgsConstructor;
import org.example.lab11.dbs.Country;
import org.example.lab11.dbs.CountryDAO;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.sql.SQLException;
import java.util.Collections;
import java.util.List;

//AllArgsConstructor
@RestController
@RequestMapping("/api/countries")
public class CountryController {
    private final CountryDAO countryDao;

    @Autowired
    public CountryController(CountryDAO countryDao) {
        this.countryDao = countryDao;
    }

    @GetMapping
    public List<Country> getAllCountries() throws SQLException {
        return Collections.singletonList(countryDao.findAll());
    }

}
