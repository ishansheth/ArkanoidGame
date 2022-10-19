#include <entity.hpp>

enum class EntityOperationTypes
  {
    ADD,
    REMOVE,
    PROCESS
  };

class EntityOperation
{
public:
  virtual void performOperation() = 0;
};

class AddOperation : public EntityOperation
{


};


  
