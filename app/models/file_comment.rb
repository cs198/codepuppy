class FileComment < ActiveRecord::Base
  belongs_to :submission_file

  validates_presence_of :submission_file_id
  validates_presence_of :line_number
  validates_presence_of :num_lines
  validates_presence_of :comment
end
