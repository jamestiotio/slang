#ifndef SLANG_COMPILER_CORE_SOURCE_MAP_H
#define SLANG_COMPILER_CORE_SOURCE_MAP_H

#include "../../slang.h"

#include "../core/slang-string.h"
#include "../core/slang-list.h"

#include "../core/slang-string-slice-pool.h"

namespace Slang {

class SourceMap : public RefObject
{
public:
    struct Entry
    {
        void init()
        {
            generatedColumn = 0;
            sourceFileIndex = 0;
            sourceLine = 0;
            sourceColumn = 0;
            nameIndex = 0;
        }

        // Note! All column/line are zero indexed
        Index generatedColumn;          ///< The generated column
        Index sourceFileIndex;          ///< The index into the source name/contents
        Index sourceLine;               ///< The line number in the originating source
        Index sourceColumn;             ///< The column number in the originating source
        Index nameIndex;                ///< Name index
    };

        /// Get the total number of generated lines
    Count getGeneratedLineCount() const { return m_lineStarts.getCount(); }
        /// Get the entries on the line
    SLANG_FORCE_INLINE ConstArrayView<Entry> getEntriesForLine(Index generatedLine) const;
    
        /// Advance to the specified line index. 
        /// It is an error to specify a line *before* the current line. It should either be the current 
        /// output line or a later output line. Interveining lines will be set as empty
    void advanceToLine(Index lineIndex);

        /// Add an entry to the current line
    void addEntry(const Entry& entry) { m_lineEntries.add(entry); }

        /// Given the slice returns the index
    Index getSourceFileIndex(const UnownedStringSlice& slice);

        /// Get the name index
    Index getNameIndex(const UnownedStringSlice& slice);

        /// Given a row and col index, find the closest entry
        /// NOTE! Zero indexed line and column.
    Index findEntry(Index lineIndex, Index colIndex) const;

        /// Given an entry index return the entry
    const Entry& getEntryByIndex(Index i) const {return m_lineEntries[i]; }

        /// Given the sourceFileIndex return the name
    UnownedStringSlice getSourceFileName(Index sourceFileIndex) const;

        /// Clear the contents of the source map
    void clear();

        /// Ctor
    SourceMap():
        m_slicePool(StringSlicePool::Style::Default)
    {
        clear();
    }

    String m_file;
    String m_sourceRoot;

    List<StringSlicePool::Handle> m_sources;

        /// Storage for the contents. Can be unset null to indicate not set. 
    List<StringSlicePool::Handle> m_sourcesContent;
    
        /// The names
    List<StringSlicePool::Handle> m_names;

    List<Index> m_lineStarts;
    List<Entry> m_lineEntries;

    StringSlicePool m_slicePool;
};

// -------------------------------------------------------------
SLANG_FORCE_INLINE ConstArrayView<SourceMap::Entry> SourceMap::getEntriesForLine(Index generatedLine) const
{
    SLANG_ASSERT(generatedLine >= 0 && generatedLine < m_lineStarts.getCount());

    const Index start = m_lineStarts[generatedLine];

    const Index end = (generatedLine + 1 >= m_lineStarts.getCount()) ? m_lineEntries.getCount() : m_lineStarts[generatedLine + 1];

    const auto entries = m_lineEntries.begin();

    SLANG_ASSERT(start >= 0 && start <= m_lineEntries.getCount());
    SLANG_ASSERT(end >= start && end >= 0 && end <= m_lineEntries.getCount());

    return ConstArrayView<SourceMap::Entry>(entries + start, end - start);
}

} // namespace Slang

#endif // SLANG_COMPILER_CORE_SOURCE_MAP_H
