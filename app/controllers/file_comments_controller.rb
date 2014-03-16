class FileCommentsController < ApiController
  skip_before_filter :verify_authenticity_token
  respond_to :json

  def index
    comments = FileComment.find_all_by_submission_file_id(
      params[:submission_file_id]
    )
    respond_with(comments)
  rescue ActiveRecord::RecordInvalid
    raise 'Submission file not found'
  end

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

  def destroy
    comment = FileComment.find(params[:id])
    comment.destroy
    render json: []
  rescue ActiveRecord::RecordNotFound
    raise 'Comment not found'
  end

  private

  def comment_params
    params.permit(
      :submission_file_id,
      :line_number,
      :num_lines,
      :comment
    )
  end
end
