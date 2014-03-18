angular.module('codepuppy').controller('FileUploaderCtrl',
    ['$scope', '$routeParams', '$fileUploader', '$http', '$location',
     function($scope, $routeParams, $fileUploader, $http, $location)
{

  $scope.uploader = $fileUploader.create({
      scope: $scope,
      url: '/submission_files.json'
  });


  $scope.uploader.bind('completeall', function(event, items) {
    console.log("ALL COMPLETED, REDIRECTING");
      var path = '/courses/' + $routeParams.courseID + '/assignments/' +
          $routeParams.assignmentID;
      $location.path(path);
      $scope.$apply();
  });

  $scope.submitAll = function()
  {
    //TODO: Fix ID to be student ID when logged in.
    $http({
      method: 'GET',
      url: '/sessions/current_person.json'
    }).success(function(data, status, headers, config) {
      console.log("SUCCESSFULLY GOT MY ID");
      console.log(data);
      var urlParams = {
        person_id: data.id,
        assignment_id: $routeParams.assignmentID,
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
        console.log("SUCCESSFULLY POSTED A SUBMISSION");
        console.log(data);

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
