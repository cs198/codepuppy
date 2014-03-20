angular.module('codepuppy').controller('LeaderSubmissionCtrl', ['$scope', '$stateParams', '$modal', '$http', '$location',
  function($scope, $stateParams, $modal, $http, $location) {

  var getFiles = function() {
    $http({method: 'GET', url: '/submissions/' + $stateParams.submissionID + '/submission_files.json'})
    .success(function(data, status, headers, config) {
      if(data.length > 0) {
        var url =  $location.$$path + '/files/' + data[0].id;
        $location.path(url);
      } else {
        $scope.message = "This submission appears to be empty.";
      }
    });
  };

  getFiles();
}]);
