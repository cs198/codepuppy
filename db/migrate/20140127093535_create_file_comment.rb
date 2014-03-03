class CreateFileComment < ActiveRecord::Migration
  def change
    create_table :file_comments do |t|
      t.belongs_to :submission_file, index: true
      t.integer :line_number
      t.integer :num_lines
      t.string :comment

      t.timestamps
    end
  end
end
