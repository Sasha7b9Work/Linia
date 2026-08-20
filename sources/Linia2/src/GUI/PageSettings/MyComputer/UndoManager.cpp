#include "defines.h"
#include "GUI/PageSettings/MyComputer/UndoManager.h"
#include <stdexcept>


UndoManager &UndoManager::GetInstance()
{
    static UndoManager instance;
    return instance;
}

void UndoManager::AddOperation(const FileOperation &operation)
{
    // Удаляем все операции после текущей позиции
    if (m_currentIndex < operations.size())
    {
        operations.erase(operations.begin() + (int64)m_currentIndex, operations.end());
    }

    operations.push_back(operation);
    m_currentIndex++;

    // Ограничиваем размер истории
    if (operations.size() > MAX_OPERATIONS)
    {
        operations.erase(operations.begin());
        m_currentIndex--;
    }
}

FileOperation UndoManager::Undo()
{
    if (!CanUndo())
    {
        throw std::runtime_error("Nothing to undo");
    }
    m_currentIndex--;
    return operations[m_currentIndex];
}

FileOperation UndoManager::Redo()
{
    if (!CanRedo())
    {
        throw std::runtime_error("Nothing to redo");
    }
    FileOperation op = operations[m_currentIndex];
    m_currentIndex++;
    return op;
}

void UndoManager::Clear()
{
    operations.clear();
    m_currentIndex = 0;
}
