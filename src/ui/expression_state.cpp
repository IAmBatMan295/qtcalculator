#include "calculator_ui.hpp"

void ExpressionState::Append_Chunk(const QString &visible_chunk, const QString &backend_chunk) {
    ExpressionChunk new_chunk;
    new_chunk.visible_chunk = visible_chunk;
    new_chunk.backend_chunk = backend_chunk;

    chunk_history_.push_back(new_chunk);
    visible_expression_ += visible_chunk;
    backend_expression_ += backend_chunk;
}

void ExpressionState::Remove_Last_Chunk() {
    if (chunk_history_.isEmpty()) {
        return;
    }

    const ExpressionChunk last_chunk = chunk_history_.back();
    chunk_history_.pop_back();

    if (visible_expression_.size() >= last_chunk.visible_chunk.size()) {
        visible_expression_.chop(last_chunk.visible_chunk.size());
    } else {
        visible_expression_.clear();
    }

    if (backend_expression_.size() >= last_chunk.backend_chunk.size()) {
        backend_expression_.chop(last_chunk.backend_chunk.size());
    } else {
        backend_expression_.clear();
    }
}

void ExpressionState::Clear() {
    visible_expression_.clear();
    backend_expression_.clear();
    chunk_history_.clear();
}

void ExpressionState::Set_Result_Text(const QString &result_text) {
    Clear();
    Append_Chunk(result_text, result_text);
}

QString ExpressionState::Get_Visible_Expression() const { return visible_expression_; }

QString ExpressionState::Get_Backend_Expression() const { return backend_expression_; }
