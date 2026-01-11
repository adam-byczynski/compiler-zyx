#ifndef SOURCEFILEPOSITIONLOCATOR_H
#define SOURCEFILEPOSITIONLOCATOR_H

#include <string>


class SourceFilePositionLocator {
public:
  int lineNumber;
  int columnNumber;
  std::string fileName;


  SourceFilePositionLocator();




};



#endif //SOURCEFILEPOSITIONLOCATOR_H
