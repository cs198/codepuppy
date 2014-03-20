angular.module('codepuppy').controller('FileUploaderCtrl',
    ['$scope', '$stateParams', '$fileUploader', '$http', '$location', '$cacheFactory',
     function($scope, $stateParams, $fileUploader, $http, $location, $cacheFactory)
{

  $scope.uploader = $fileUploader.create({
      scope: $scope,
      url: '/submission_files.json'
  });


  $scope.uploader.bind('completeall', function(event, items) {
      var path = '/courses/' + $stateParams.courseID + '/assignments/' +
          $stateParams.assignmentID;
      var $httpDefaultCache = $cacheFactory.get('$http');

      $location.path(path);
      $scope.$apply();
  });

  $scope.submitAll = function()
  {
    $http({
      method: 'GET',
      url: '/sessions/current_person.json'
    }).success(function(data, status, headers, config) {
      var urlParams = {
        person_id: data.id,
        assignment_id: $stateParams.assignmentID,
        feedback_released: false,
        date_submitted: (new Date()).toJSON()
      };

      // 4 URL params: assignment_id, person_id, date_submited,
      // feedback_released
      $http({
        method: 'POST',
        url: '/submissions.json',
        data: urlParams
      }).success(function(data, status, headers, config) {
        // For each item we upload, we send off a POST request to
        // submission_files#create. data is response from the POST
        // request to submissions#create, which contains the
        // submission_id.

        var items = $scope.uploader.getNotUploadedItems();

        for (var i = 0; i < items.length; i++) {
          // Adds submission_id to the params for each item's
          // POST request to submission_files#create
          items[i].formData = [{'submission_id': data.id}];
        }

        // Submit each member of the queue
        $scope.uploader.uploadAll();
      });
    });
  };
}]);
