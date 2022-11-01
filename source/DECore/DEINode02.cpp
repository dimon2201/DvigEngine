#include "../../include/DECore.hpp"

// void DvigEngine2::INode::AddChildNode(INode* node)
// {
//     this->m_ChildNodes->Insert((const char*)node->GetSID(), (void*)node);
// }

// void DvigEngine2::INode::RemoveChildNode(const char* SID)
// {
//     this->m_ChildNodes->Remove(&SID[0]);
// }

void DvigEngine2::ICommon::SetSIDAndCreateeAndMemoryObjectAndEngine(deuchar* SID, ICommon** createe, MemoryObject** memoryObject, Engine* engine)
{
    // deisize m_SIDByteWidth = 0;
    // while (SID[m_SIDByteWidth] != 0) { this->m_SID[m_SIDByteWidth] = SID[m_SIDByteWidth]; ++m_SIDByteWidth; }
    // this->m_SID[m_SIDByteWidth] = 0;
    // this->m_IID = IID;

    this->m_Createe = createe;
    this->m_MemoryObject = memoryObject;
    this->m_Engine = engine;
    // this->m_ParentNode = nullptr;
    // DvigEngine2::HashMap* hms[1];
    // engine->NodeCreate<DvigEngine2::HashMap>( &hms, "_ChildNodesHashMap", nullptr );
    // this->m_ChildNodes->m_IID = Engine::GetGlobalIID();
    // this->m_ChildNodes->Init( 1024, sizeof(DvigEngine2::HashMapKeyValuePair), 1024 );
}