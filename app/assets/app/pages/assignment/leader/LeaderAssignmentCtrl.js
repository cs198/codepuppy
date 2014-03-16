angular.module('codepuppy').controller('LeaderAssignmentCtrl', ['$scope', '$routeParams', '$fileUploader', '$http', '$sce', function($scope, $routeParams, $fileUploader, $http, $sce) {

  var getAssignment = function() {
    $http({method: 'GET', url: '/assignments/' + $routeParams.assignmentID + '.json'})
    .success(function(data, status, headers, config) {
      if (data.name) {
        $scope.assignmentName = data.name;
      } else {
        $scope.assignmentName = "";
      }

      if (data.description) {
        if (true || iswiki(data.description)) {
          $scope.assignmentDescription = $sce.trustAsHtml(
            wiki2html(data.description)
          );
        } else {
          $scope.assignmentDescription = data.description;
        }
      } else {
        $scope.assignmentDescription = "";
      }
      console.log($scope.assignmentDescription);
    });
  };
  getAssignment();

  $scope.message = "I'm an assignment page for the course " + $routeParams.courseID;
  $scope.message += " and assignment " + $routeParams.assignmentID;
}]);
