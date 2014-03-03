class SubmissionFile < ActiveRecord::Base
  belongs_to :submission

  validates_presence_of :submission_id
  validates_presence_of :data
  validates_presence_of :data
end
