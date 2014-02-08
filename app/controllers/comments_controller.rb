class CommentsController < ApplicationController

  def create
    Comment.create(comment_params)
  end

  private

    def comment_params
      params.require(:comment).permit(:submission_id, :file_location, :line_number, :num_lines, :comment)
    end

end
