class CreateSubmissionFile < ActiveRecord::Migration
  def change
    create_table :submission_files do |t|
      t.belongs_to :submission, index: true
      t.string :path
      t.text :data
    end
  end
end
