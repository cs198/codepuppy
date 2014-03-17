class AnnouncementsController < ApiController
  skip_before_filter :verify_authenticity_token
  respond_to :json

  def index
    if params.key?(:course_id)
      announcements = Announcement.find_all_by_course_id(params[:course_id])
      respond_with(announcements)
    else
      respond_with([])
    end
      rescue ActiveRecord::RecordNotFound
    raise 'Course not found from course_id'
  end

  def create
    announcement = Announcement.create(announcement_params)
    puts announcement.inspect
    respond_with(announcement)
  rescue ActiveRecord::RecordInvalid => invalid
    puts invalid.record.errors
  end

  def destroy
    announcement = Announcement.find(params[:id])
    announcement.destroy
    render json: []
  rescue ActiveRecord::RecordNotFound
    raise 'Announcement not found'
  end

  private

  def announcement_params
    params.permit(:course_id, :subject, :body)
  end
end
