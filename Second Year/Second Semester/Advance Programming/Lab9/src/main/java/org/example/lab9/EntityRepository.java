package org.example.lab9;

import jakarta.persistence.EntityManager;
import jakarta.persistence.TypedQuery;

import java.util.List;

public class EntityRepository {
    public void create(MyEntity entity) {
        EntityManager em = DBSingleton.getEntityManagerFactory().createEntityManager();
        em.getTransaction().begin();
        em.persist(entity);
        em.getTransaction().commit();
        em.close();
    }

    public MyEntity findById(Long id) {
        EntityManager em = DBSingleton.getEntityManagerFactory().createEntityManager();
        MyEntity entity = em.find(MyEntity.class, id);
        em.close();
        return entity;
    }

    public List<MyEntity> findByName(String namePattern) {
        EntityManager em = DBSingleton.getEntityManagerFactory().createEntityManager();
        TypedQuery<MyEntity> query = em.createNamedQuery("MyEntity.findByName", MyEntity.class);
        query.setParameter("name", namePattern);
        List<MyEntity> results = query.getResultList();
        em.close();
        return results;
    }
}