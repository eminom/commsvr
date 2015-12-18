

#include "MakeDirTask.h"
#include "files/DirMaker.h"

MakeDirTask::~MakeDirTask(){

}

MakeDirTask::MakeDirTask(const std::string &targetDir)
	:_targetDir(targetDir){
}

void MakeDirTask::Proceed(){
	MkdirRecur(_targetDir);
	delete this;
}
