class Comment < ActiveRecord::Base
  belongs_to :submission

  validates_presence_of :submission_id
  validates_presence_of :file_location
  validates_presence_of :line_number
  validates_presence_of :num_lines
  validates_presence_of :comment
end
