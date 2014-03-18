angular.module('codepuppy').controller('LeaderSubmissionFileCtrl', ['$scope', '$routeParams', '$modal', '$http', 
  function($scope, $routeParams, $modal, $http) {
    
  $scope.message = 'I\'m a submission page for the course ' +
    $routeParams.courseID;
  $scope.message += ' and assignment ' + $routeParams.assignmentID;
  $scope.message += ' and submission ' + $routeParams.submissionID;

  // Map from line numbers to commentBody objects -- have .comment and .lineNumber fields
  $scope.comments = {};
  $scope.commentsReleased = false;
  $scope.commentPermissions = true;
  $scope.files = [];

  var getSubmission = function() {
    $http({method: 'GET', url: '/submissions/' + $routeParams.submissionID + '.json'})
    .success(function(data, status, headers, config) {
      $scope.commentsReleased = data.submission.feedback_released;
    });
  };

  var getFiles = function() {
    $http({method: 'GET', url: '/submissions/' + $routeParams.submissionID + '/submission_files.json'})
    .success(function(data, status, headers, config) {
      for(var i = 0; i < data.length; ++i) {
        $scope.files.push(data[i]);
        if(data[i].id == $routeParams.submissionFileID) {
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
          url: '/submissions/' + $routeParams.submissionID + '/update_feedback_released.json',
          data: urlParams })
        .success(function(data, status, headers, config) {
        });
      };

      toggleFeedbackReleased();
    }
  });

}]);