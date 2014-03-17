class Announcement < ActiveRecord::Base
  belongs_to :course

  validates_presence_of :course_id
  validates_presence_of :subject
  validates_presence_of :body
end
