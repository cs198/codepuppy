class FileCommentsController < ApplicationController
  skip_before_filter :verify_authenticity_token
  respond_to :html, :xml, :json

  def create
    comment = FileComment.create(comment_params)
    respond_with(comment)
  rescue ActiveRecord::RecordInvalid => invalid
    puts invalid.record.errors
  end

  def show
    comment = FileComment.find(params[:id])
    respond_with(comment)
  rescue ActiveRecord::RecordNotFound
    raise 'Comment not found'
  end

  private

  def comment_params
    params.permit(
      :submission_id,
      :file_location,
      :line_number,
      :num_lines,
      :comment
    )
  end
end
