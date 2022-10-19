#ifndef ENTITYCOLLECTION_HPP_
#define ENTITYCOLLECTION_HPP_

#include "entity.hpp"
#include <memory>

template<typename T>
class EntityCollection : public Entity
{
protected:
  std::vector<std::shared_ptr<T>> entityContainer;
public:

  class iterator
  {
    std::shared_ptr<T> *ptr;
  public:
    iterator(std::shared_ptr<T>* p):ptr(p){}
    iterator operator++()
    {
      ++ptr;
      return *this;
    }

    bool operator!=(const iterator& other) 
    {
      return ptr != other.ptr;
    }

    std::shared_ptr<T> operator*() const
    {
      return *ptr;
    }
  };
  
  EntityCollection():entityContainer(){}

  template<typename...Args>
  void addEntity(Args&&...args)
  {
    entityContainer.emplace_back(std::make_shared<T>(std::forward<Args&&>(args)...));
  }

  T* getEntityAt(int i)
  {
    if(i < entityContainer.size() && i != 0)
      return entityContainer[i].get();
    else
      return nullptr;
  }

  std::size_t getCollectionSize(){return entityContainer.size();}

  void clearCollection()
  {
    entityContainer.clear();
  }

  virtual void createCollection() = 0;
  
};


#endif
