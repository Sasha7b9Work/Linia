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
    if (current_index < operations.size())
    {
        operations.erase(operations.begin() + (int64)current_index, operations.end());
    }

    operations.push_back(operation);
    current_index++;

    // Ограничиваем размер истории
    if (operations.size() > MAX_OPERATIONS)
    {
        operations.erase(operations.begin());
        current_index--;
    }
}

FileOperation UndoManager::Undo()
{
    if (!CanUndo())
    {
        throw std::runtime_error("Nothing to undo");
    }
    current_index--;
    return operations[current_index];
}

FileOperation UndoManager::Redo()
{
    if (!CanRedo())
    {
        throw std::runtime_error("Nothing to redo");
    }
    FileOperation op = operations[current_index];
    current_index++;
    return op;
}

void UndoManager::Clear()
{
    operations.clear();
    current_index = 0;
}
