angular.module('codepuppy').controller('LeaderSubmissionFileCtrl', ['$scope', '$stateParams', '$modal', '$http',
  function($scope, $stateParams, $modal, $http) {

  $scope.message = 'I\'m a submission page for the course ' +
    $stateParams.courseID;
  $scope.message += ' and assignment ' + $stateParams.assignmentID;
  $scope.message += ' and submission ' + $stateParams.submissionID;

  $scope.comments = {};
  $scope.commentsReleased = false;
  $scope.commentPermissions = true;
  $scope.files = [];

  var getSubmission = function() {
    $http({method: 'GET', url: '/submissions/' + $stateParams.submissionID + '.json'})
    .success(function(data, status, headers, config) {
      $scope.commentsReleased = data.submission.feedback_released;
    });
  };

  var getFiles = function() {
    $http({method: 'GET', url: '/submissions/' + $stateParams.submissionID + '/submission_files.json'})
    .success(function(data, status, headers, config) {
      for(var i = 0; i < data.length; ++i) {
        $scope.files.push(data[i]);
        if(data[i].id == $stateParams.submissionFileID) {
          $scope.selectedFile = data[i];
        }
      }
    });
  };

  getFiles();
  getSubmission();

  $scope.$watch('commentsReleased', function(oldValue, newValue) {
    if(oldValue != newValue) {
      var urlParams = {
        feedback_released: $scope.commentsReleased,
      };

      var toggleFeedbackReleased = function() {
        $http({method: 'PUT',
          url: '/submissions/' + $stateParams.submissionID + '/update_feedback_released.json',
          data: urlParams })
        .success(function(data, status, headers, config) {
        });
      };

      toggleFeedbackReleased();
    }
  });

}]);
