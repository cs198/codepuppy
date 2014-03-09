class ChangePathToFilenameInSubmissionFiles < ActiveRecord::Migration
  def change
    rename_column :submission_files, :path, :original_filename
  end
end
