Namepending::Application.routes.draw do
  resources :submission_files

  root 'index#show'

  resources :courses do
    collection do
      get 'active'
    end

    get 'students'
    get 'leaders'
    get 'admins'
    resources :assignments
    resources :announcements
  end

  resources :assignments do
    resources :submissions
  end

  resources :people do
    get 'courses'
  end

  resources :sections do
    get 'leaders'
    get 'students'
  end

  resources :submissions do
    resources :submission_files

    put 'update_feedback_released'
  end

  resources :submission_files do
    resources :file_comments
  end

  resources :file_comments
  resources :announcements

  post '/sessions/generate', to: 'sessions#generate'
  post '/sessions/verify', to: 'sessions#verify'
  get '/sessions/verify', to: 'index#verify_session'
  get '/sessions/current_person', to: 'sessions#current_person'

  # The priority is based upon order of creation: first created -> highest priority.
  # See how all your routes lay out with "rake routes".

  # You can have the root of your site routed with "root"
  # root 'welcome#index'

  # Example of regular route:
  #   get 'products/:id' => 'catalog#view'

  # Example of named route that can be invoked with purchase_url(id: product.id)
  #   get 'products/:id/purchase' => 'catalog#purchase', as: :purchase

  # Example resource route (maps HTTP verbs to controller actions automatically):
  #   resources :products

  # Example resource route with options:
  #   resources :products do
  #     member do
  #       get 'short'
  #       post 'toggle'
  #     end
  #
  #     collection do
  #       get 'sold'
  #     end
  #   end

  # Example resource route with sub-resources:
  #   resources :products do
  #     resources :comments, :sales
  #     resource :seller
  #   end

  # Example resource route with more complex sub-resources:
  #   resources :products do
  #     resources :comments
  #     resources :sales do
  #       get 'recent', on: :collection
  #     end
  #   end

  # Example resource route with concerns:
  #   concern :toggleable do
  #     post 'toggle'
  #   end
  #   resources :posts, concerns: :toggleable
  #   resources :photos, concerns: :toggleable

  # Example resource route within a namespace:
  #   namespace :admin do
  #     # Directs /admin/products/* to Admin::ProductsController
  #     # (app/controllers/admin/products_controller.rb)
  #     resources :products
  #   end
end
