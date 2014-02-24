class CommentsController < ApplicationController
  respond_to :html, :xml, :json

  def create
    Comment.create(comment_params)
  end

  def show
    comment = Comment.find(params[:id])
    respond_with(comment)
  rescue ActiveRecord::RecordNotFound
    raise 'Comment not found'
  end

  private

  def comment_params
    params.require(:comment).permit(
      :submission_id,
      :file_location,
      :line_number,
      :num_lines,
      :comment
    )
  end
end
