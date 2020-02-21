#pragma once

#include "reader.h"
#include "dictionary.h"
#include "event.h"
#include "scaler.h"
#include <string>

namespace clas12 {

  class scaler_reader{

  public:
    scaler_reader(std::string filename);

    double getBeamCharge() const noexcept{return _maxCup;}
    
  private:
    //reader
    hipo::reader     _reader;
    hipo::event      _event;
    hipo::dictionary  _factory;

    scaler_uptr _bscal;
    double _maxCup{0};

  };

}