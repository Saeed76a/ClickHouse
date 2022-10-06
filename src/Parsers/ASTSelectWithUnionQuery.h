#pragma once

#include <Parsers/ASTQueryWithOutput.h>
#include <Parsers/SelectUnionMode.h>

namespace DB
{
/** Single SELECT query or multiple SELECT queries with UNION
 * or UNION or UNION DISTINCT
  */
class ASTSelectWithUnionQuery : public ASTQueryWithOutput
{
public:
    String getID(char) const override { return "SelectWithUnionQuery"; }

    ASTPtr clone() const override;

    void formatQueryImpl(const FormatSettings & settings, FormatState & state, FormatStateStacked frame) const override;

    QueryKind getQueryKind() const override { return QueryKind::Select; }

    SelectUnionMode union_mode;

    SelectUnionModes list_of_modes;

    bool is_normalized = false;

    ASTPtr list_of_selects;

    SelectUnionModesSet set_of_modes;

    /// Consider any mode other than ALL as non-default.
    bool hasNonDefaultUnionMode() const;

    bool has_query_parameters = false;
    bool hasQueryParameters() const { return has_query_parameters; }
    void setHasQueryParameters();

    //clang-tidy wants it to be const, but it changes flags of children
    void clearAllowQueryParameters(); // NOLINT

};

}
