class CreateComment < ActiveRecord::Migration
  def change
    create_table :comments do |t|
      t.belongs_to :submission, index: true
      t.string :file_location
      t.integer :line_number
      t.integer :num_lines
      t.string :comment
    end

    add_index :comments, :file_location
  end
end
