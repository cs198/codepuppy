angular.module('codepuppy').controller('FileUploaderCtrl',
['$scope', '$routeParams', '$fileUploader', '$http', '$location', function($scope, $routeParams, $fileUploader, $http, $location) {

  $scope.uploader = $fileUploader.create({
      scope: $scope,
      url: '/submission_files.json',
      formData: [{'key': 'value'}],
  });


  $scope.uploader.bind('completeall', function (event, items) {
      var path = '/' + $routeParams.courseID + '/' + $routeParams.assignmentID;
      $location.path(path);
      $scope.$apply();
  });

  $scope.submitAll = function()
  {
    var createSubmission = function() {
      //TODO: Fix ID to be student ID when logged in.
      var date = new Date();
      var urlParams = {
        person_id: 1,
        assignment_id: $routeParams.assignmentID,
        feedback_released: false,
        date_submitted: date.toJSON()
      };


      // 4 URL params: assignment_id, person_id, date_submited, feedback_released
      $http({method: 'POST', url: '/submissions.json', data: urlParams}).success(function(data, status, headers, config) {
        console.log(data);
        // Add submission ID to the file data

        var items = $scope.uploader.getNotUploadedItems();
        console.log(items);
        for (var i = 0; i < items.length; i++) {
          items[i].formData = [{'submission_id': data.id}];
          console.log("item");
          console.log(items[i]);
        }
        // Submit each member of the queue
        $scope.uploader.uploadAll();
      });

    };

    createSubmission();
  };

}]);
