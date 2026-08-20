#pragma once
#pragma warning(push, 0)
    #include <wx/arrstr.h>
#pragma warning(pop)


enum class OperationType
{
    COPY,
    MOVE,
    _DELETE,
    CREATE_FOLDER
};

struct FileOperation
{
    OperationType type;
    wxArrayString files;
    wxString sourcePath;
    wxString destPath;

    FileOperation(OperationType t, const wxArrayString &f,const wxString &src, const wxString &dst = ""):
        type(t), files(f), sourcePath(src), destPath(dst)
    {
    }
};

class UndoManager
{
public:
    static UndoManager &GetInstance();

    void AddOperation(const FileOperation &operation);
    bool CanUndo() const
    {
        return current_index > 0;
    }
    bool CanRedo() const
    {
        return current_index < operations.size();
    }

    FileOperation Undo();
    FileOperation Redo();
    void Clear();

private:
    UndoManager() : current_index(0) { }

    std::vector<FileOperation> operations;
    size_t current_index;
    static const size_t MAX_OPERATIONS = 50;
};
