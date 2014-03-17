class AddPdfUrlToAssignments < ActiveRecord::Migration
  def up
    add_column :assignments, :pdf_url, :string
  end
  def down
    remove_column :assignments, :pdf_url
  end
end
